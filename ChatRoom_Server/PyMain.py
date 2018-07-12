# -*- coding:utf-8 -*-
import CServerSocket

HOST = '127.0.0.1'
PORT = 1234
if __name__ == "__main__":
    server = CServerSocket.CServerSocket(HOST, PORT)
    server.myAccept();

