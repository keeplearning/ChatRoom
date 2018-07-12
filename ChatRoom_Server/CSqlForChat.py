# -*- coding:utf-8 -*-
import mysql.connector
import traceback

class CSqlForChat():
    def __init__(self):
        #1.链接数据库
        config = {'host':'127.0.0.1',
                  'user':'root',
                  'password':'root',
                  'port':'3306',
                  'database':'chatroom',
                  'charset':'utf8'}
        print("正在链接数据库...")
        self.conn = mysql.connector.connect(**config)
        print("数据库链接成功")
    def __del__(self):
        self.conn.close()
    def query(self, szSql, param = None):
        cursor = self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result = (cursor.fetchall(), cursor.rowcount)
            cursor.close();
            return result
        except Exception as err:
            print(err)
            cursor.close()
            self.conn.rollback()
            return None

    def insert(self, szSql, param = None):
        cursor = self.conn.cursor()
        try:
            cursor.execute(szSql, param)
            result = cursor.rowcount
            self.conn.commit()
            cursor.close()
            return result
        except Exception as err:
            msg = traceback.format_exc()
            print(msg)
            cursor.close()
            self.conn.rollback()
            return None

