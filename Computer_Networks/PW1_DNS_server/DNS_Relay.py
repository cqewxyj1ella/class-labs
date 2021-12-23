import socket
import threading

class DNS_Relay_Server:      #一个relay server实例，通过缓存文件和外部地址来初始化
    def __init__(self,cache_file,name_server):
        #url_IP字典:通过域名查询ID
        self.url_ip = {}
        self.cache_file = cache_file
        self.load_file()
        self.name_server = name_server
        #trans字典：通过DNS响应的ID来获得原始的DNS数据包发送方
        self.trans = {}    

    def load_file(self,):
        f = open(self.cache_file,'r',encoding='utf-8')
        for line in f:
            ip,name = line.split(' ')
            self.url_ip[name.strip('\n')] = ip
        f.close()

    def run(self):
        buffer_size = 512
        server_socket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind(('',53))
        server_socket.setblocking(False)
        while True:
            try:
                data,addr = server_socket.recvfrom(buffer_size)
                threading.Thread(target=self.handle,args=(server_socket,data,addr)).start()
            except:
                continue 

    def handle(self,server_socket,data,addr):
        RecvDp = DNS_Packege(data)
        print("handle()")
        #print("data: {}".format(data))
        print("Dict's length: {}".format(len(self.url_ip)))
        #print(self.url_ip)
        print(RecvDp.name_length)
        ip_lookup = self.url_ip.get(RecvDp.name, None)
        #print(self.url_ip.get(RecvDp.name, "Not found in configuration file"))
        if RecvDp.QR == 0:  # query TODO:
            print("query")
            if ip_lookup is not None:
                print(ip_lookup)
                if ip_lookup != "0.0.0.0":
                    print("not Intercepted")
                    res = RecvDp.generate_response(ip_lookup, False)
                    print(res)
                else:
                    print("Intercepted")
                    res = RecvDp.generate_response("0.0.0.0", True)
                    print(res)
            else:
                print("Not Found")
        else:               # response TODO:
            print("response")
            print(data)

class DNS_Packege:        #一个DNS Frame实例，用于解析和生成DNS帧
    def __init__(self,data):
        self.data = data
        Msg_arr = bytearray(data)
        #ID
        self.ID = (Msg_arr[0] << 8 ) + Msg_arr[1]
        # FLAGS
        self.QR = Msg_arr[2] >> 7
        # 资源记录数量
        self.QDCOUNT = (Msg_arr[4] << 8) + Msg_arr[5]
        #query内容解析
        self.name = ""
        self.name_length = 0
        indx = 12
        get = Msg_arr[indx]
        #print("type: {}".format(type(get)))
        while get != 0x00:
            print("get: {}".format(get))
            self.name_length += get
            while get > 0:
                indx += 1
                print(chr(Msg_arr[indx]))
                self.name += chr(Msg_arr[indx])
                get -= 1
            self.name += '.'
            indx += 1
            get = Msg_arr[indx]
        self.name = self.name[:len(self.name)-1]
        # strip the last "." in the string
        print(self.name)

    def generate_response(self,ip,Intercepted):
        if not Intercepted:
            res = bytearray(32 + self.name_length)
            res[0] = self.ID >> 8
            res[1] = self.ID % 256
            #......TODO:
            for i in range(12, 16 + self.name_length):
                res[i] = self.data[i]
            #......TODO:
            return bytes(res)
        else:
            print("else")   #temporaryTODO:

   

if __name__ == '__main__':
    cache_file = 'example.txt'
    name_server=('127.0.0.53',53)
    relay_server = DNS_Relay_Server(cache_file,name_server)   #构造一个DNS_Relay_Server实例
    relay_server.run()