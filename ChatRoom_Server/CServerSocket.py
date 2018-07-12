# -*- coding:utf-8 -*-
import socket
import threading
import struct
from enum import Enum
from CSqlForChat import CSqlForChat
import time

class EnumMessageType(Enum):
    ANONYMOUS = 1
    CHAT = 2
    ONE2ONE = 3
    REGISTER = 4
    LOGIN = 5
    ADDFRIEND = 6
    SEARCHUSER = 7
    FILETRANS = 8
    MSGRECORD = 9
    UPDATEUSER = 10

class CServerSocket():
    #类变量
    conn = CSqlForChat()
    def __init__(self, ip, port):
        ADDR = (ip, port)
        #初始化socket
        print("正在启动服务器...")
        self.socketServer = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socketServer.bind(ADDR)
        self.socketServer.listen(5)
        print("服务器启动成功，等待客户端链接...")

    def myAccept(self):
        #创建线程
        t = threading.Thread(target=self.__acceptProc__)
        t.start()
    #accept的回调函数
    def __acceptProc__(self):
        while True:
            #accept返回的是个元组
            socketClient, addrClient = self.socketServer.accept()
            print("接收到客户端链接")
            t = threading.Thread(target=self.__recvProc__,
                                 args=(socketClient,))
            t.start()
    #接收消息的线程
    def __recvProc__(self, s):
        while True:
            try:
                print("recv回调函数")
                message = s.recv(CServerSocket.BUFSIZE+10)
                #消息类型
                type, = struct.unpack("i", message[:4])
                CServerSocket.dictFun[type](s, message)
            except Exception as err:
                print(err)
                name = CServerSocket.dictClient.get(s)
                if name == None:
                    return
                s.close()
                name = CServerSocket.dictClient.pop(s)
                #CServerSocket.UpdateUser(s, False, name)
                return
    #不同聊天目的的回调函数
    def __ChatForAnonymous__(s, msg):
        dwLen, = struct.unpack("L", msg[4:8])
        roomid, = struct.unpack("L", msg[8:12])
        buf, = struct.unpack("%ds" % dwLen, msg[12:12 + dwLen])
        name = buf.decode("gb2312").rstrip('\0')
        CServerSocket.dictClient[s] = name
        for each in CServerSocket.dictClient:
            each.send(msg)
        #将用户名字加入到聊天室list中
        CServerSocket.AddRoomUser(roomid, name)
        #通知给每个客户端在同一个聊天室更新在线用户列表
        CServerSocket.UpdateUser(s, True, name, roomid)
    def __ChatForLeave__(s, msg):
        dwLen, = struct.unpack("L", msg[4:8])
        roomid, = struct.unpack("L", msg[8:12])
        buf, = struct.unpack("%ds" % dwLen, msg[12:12 + dwLen])
        name = buf.decode("gb2312").rstrip('\0')
        CServerSocket.DelRoomUser(roomid, name)
        CServerSocket.UpdateUser(s, False, name, roomid)
    @staticmethod
    def DelRoomUser(roomid, name):
        if roomid == 10001:
            CServerSocket.listRoom1.remove(name)
        elif roomid == 10002:
            CServerSocket.listRoom2.remove(name)
        elif roomid == 10003:
            CServerSocket.listRoom3.remove(name)
    @staticmethod
    def AddRoomUser(roomid, name):
        if roomid == 10001:
            CServerSocket.listRoom1.append(name)
        elif roomid == 10002:
            CServerSocket.listRoom2.append(name)
        elif roomid == 10003:
            CServerSocket.listRoom3.append(name)
    @staticmethod
    def UpdateUser(s, bAdd, name, roomid):
        try:
            #先给所有客户端的用户列表里添加新用户
            message_type = EnumMessageType.UPDATEUSER
            message = name.encode("gb2312")
            message_len = len(message)
            message_send = struct.pack("llll2040s", message_type, bAdd, message_len, roomid,message)
            for each in CServerSocket.dictClient:
                if each == s:
                    continue
                each.send(message_send)
                time.sleep(0.01)
            if bAdd == False:
                return
            #再给新用户的在线列表添加之前的登陆用户
            for each in CServerSocket.dictClient:
                if each == s:
                    continue
                message = CServerSocket.dictClient[each].encode("gb2312")
                bsend = CServerSocket.IsSendList(roomid, message)
                if (bsend):
                    message_len = len(message)
                    message_send = struct.pack("llll2040s", message_type, bAdd, message_len, roomid, message)
                    s.send(message_send)
                    time.sleep(0.01)
        except Exception as err:
            print(err)
            return
    @staticmethod
    def IsSendList(roomid, name):
        if roomid == 10001:
            if name in CServerSocket.listRoom1:
                return True
        elif roomid == 10002:
            if name in CServerSocket.listRoom2:
                return True
        elif roomid == 10003:
            if name in CServerSocket.listRoom3:
                return True
        list1 = CServerSocket.listRoom1
        print("聊天室1")
        print(list1)
        list2 = CServerSocket.listRoom2
        print("聊天室2")
        print(list2)
        list3 = CServerSocket.listRoom3
        print("聊天室3")
        print(list3)
        return False
    def __ChatForChat__(s, msg):
        dwLen, = struct.unpack("L", msg[4:8])
        buf, = struct.unpack("%ds" % dwLen, msg[12:12 + dwLen])
        message_recv = buf.decode("gb2312")
        print(message_recv)
        for each in CServerSocket.dictClient:
            if each == s:
                continue
            each.send(msg)
    def __ChatForOne2One__(s, msg):
        userfrom, = struct.unpack("50s", msg[8:58])
        userto, = struct.unpack("50s", msg[58:108])
        userfrom = userfrom.decode("gb2312").rstrip('\0')
        userto = userto.decode("gb2312").rstrip('\0')
        for each in CServerSocket.dictClient:
            if userto == CServerSocket.dictClient[each]:
                each.send(msg)
        #消息内容
        msgcontent, = struct.unpack("1024s", msg[108:108+1024])
        msgcontent = msgcontent.decode("gb2312").rstrip('\0')
        #把消息添加到数据库
        CServerSocket.conn.insert("insert into msginfo(userfrom,userto,msgcontent) VALUES(%s,%s,%s)",
                                  (userfrom, userto, msgcontent))
    def __ChatForLogin__(s, msg):
        name, = struct.unpack("50s", msg[4:54])
        print(name)
        pwd, = struct.unpack("50s", msg[54:104])
        print(pwd)
        name = name.decode('gb2312').rstrip('\0')
        pwd = pwd.decode('gb2312').rstrip('\0')
        #构造查询语句
        result = CServerSocket.conn.query("SELECT * from userinfo where name=%s and pwd=%s",(name,pwd))
        #返回登录结果
        message_type = EnumMessageType.LOGIN
        message_len = 50
        message = ""
        if result == None or result[1] == 0:
            message = "fail".encode('gb2312')
        else:
            message = "success".encode('gb2312')
        message_send = struct.pack("l2048s", message_type, message)
        s.send(message_send)
    def __ChatForRegister__(s, msg):
        name, = struct.unpack("50s", msg[4:54])
        print(name)
        pwd, = struct.unpack("50s", msg[54:104])
        print(pwd)
        name = name.decode('gb2312').rstrip('\0')
        pwd = pwd.decode('gb2312').rstrip('\0')
        #构造查询语句
        result = CServerSocket.conn.insert("insert into userinfo(name,pwd) VALUES(%s,%s)",(name, pwd))
        message_type = EnumMessageType.REGISTER
        message_len = 50
        message = ""
        if result == None:
            message = "fail".encode('gb2312')
        else:
            message = "success".encode('gb2312')
        message_send = struct.pack("l2048s", message_type, message)
        s.send(message_send)
    def __ChatForAddFriend__(s, msg):
        #获取要添加好友的双方姓名
        name, = struct.unpack("50s", msg[4:54])
        frd, = struct.unpack("50s", msg[54:104])
        name = name.decode('gb2312').rstrip('\0')
        frd = frd.decode('gb2312').rstrip('\0')
        #构造查询语句
        result = CServerSocket.conn.insert("insert into userfriend(name,friend) VALUES(%s,%s)",(name,frd))
        #返回查询结果
        message_type = EnumMessageType.ADDFRIEND
        message_len = 50
        message = ""
        if result == None:
            message = "fail".encode('gb2312')
        else:
            message = "success".encode('gb2312')
        message_send = struct.pack("l2048s", message_type, message)
        s.send(message_send)
    def __ChatForSearUser__(s, msg):
        #要查询的名字
        name, = struct.unpack("50s", msg[4:54])
        name = name.decode('gb2312').rstrip('\0')
        #构造查询语句
        result = CServerSocket.conn.query("SELECT name from userinfo where name=%s",(name,))
        #返回搜索结果
        message_type = EnumMessageType.SEARCHUSER
        message_len = 50
        message = ""
        if result == None or result[1] == 0:
            message = "nobody".encode('gb2312')
        else:
            #是否在线
            if name in CServerSocket.dictClient.values():
                message = "online".encode('gb2312')
            else:
                message = "offline".encode('gb2312')
        message_send = struct.pack("l2048s", message_type, message)
        s.send(message_send)
    def __ChatForGetMsgRecord__(s, msg):
        name = CServerSocket.dictClient[s]
        #查询所有信息
        result = CServerSocket.conn.query("select * from msginfo where userfrom=%s or userto=%s",
                                          (name,name))
        if result == None or result[1] == 0:
            return
        message_type = EnumMessageType.MSGRECORD
        for i in range(result[1]):
            msgfrom = result[0][i][1].encode('gb2312')
            msgto = result[0][i][2].encode('gb2312')
            msgcontent = result[0][i][3].encode('gb2312')
            msgsend = struct.pack("l50s50s1948s", message_type, msgfrom, msgto, msgcontent)
            s.send(msgsend)
        #最后发个end表示数据全部发放完毕
        msgfrom = "~~~end~~~".encode('gb2312')
        msgsend = struct.pack("l2048s",message_type, msgfrom)
        s.send(msgsend)
    #类变量
    dictFun = {
        1:__ChatForAnonymous__,
        2:__ChatForChat__,
        3:__ChatForOne2One__,
        4:__ChatForRegister__,
        5:__ChatForLogin__,
        6:__ChatForAddFriend__,
        7:__ChatForSearUser__,
        9:__ChatForGetMsgRecord__,
        11:__ChatForLeave__,
    }
    BUFSIZE = 2048 + 4
    dictClient = {}
    listRoom1 = []
    listRoom2 = []
    listRoom3 = []