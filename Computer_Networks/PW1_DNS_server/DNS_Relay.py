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
        #server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
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
        print("addr: {}".format(addr))
        print("data: {}".format(data))
        print("Dict's length: {}".format(len(self.url_ip)))
        ip_lookup = self.url_ip.get(RecvDp.name, None)
        local_addr = ('localhost', 53)
        if RecvDp.QR == 0:  # query
            print("query")
            self.trans[RecvDp.ID] = addr
            if ip_lookup is not None:
                print(ip_lookup)
                if ip_lookup != "0.0.0.0":
                    print("not Intercepted")
                    res = RecvDp.generate_response(ip_lookup, False)
                    print("res: {}".format(res))
                    sent = server_socket.sendto(res, addr)
                    print('sent {} bytes back to {}'.format(sent, addr))
                else:
                    print("Intercepted")
                    res = RecvDp.generate_response("0.0.0.0", True)
                    print("res tranverted: {}".format(res))
                    sent = server_socket.sendto(res, addr)
                    print('sent {} bytes back to {}'.format(sent, addr))
            else:
                print("Not Found in the configuration file")
                sent = server_socket.sendto(data, self.name_server)
                print('sent {} bytes back to {}'.format(sent, self.name_server))
        else:               # response
            print("!!!!!!!response")
            if ip_lookup is None:
                local_addr = self.trans[RecvDp.ID]
                sent = server_socket.sendto(data, local_addr)
                print('sent {} bytes back to {}'.format(sent, local_addr))

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
            self.name_length += get+1
            while get > 0:
                indx += 1
                self.name += chr(Msg_arr[indx])
                get -= 1
            self.name += '.'
            indx += 1
            get = Msg_arr[indx]
        self.name_length += 1
        print("self.name_length: {}".format(self.name_length))
        self.name = self.name[:len(self.name)-1]
        # strip the last "." in the string
        print(self.name)

    def generate_response(self,ip,Intercepted):
        Msg_arr = bytearray(self.data)
        res = bytearray(32 + self.name_length)
        res[0] = self.ID >> 8
        res[1] = self.ID % 256
        # before query name
        res[2] = 0x81   # QR = 1; RD = 1
        res[3] = 0x80   # RA = 1
        res[4] = 0x00
        res[5] = 0x01   # QDCOUNT
        res[6] = 0x00
        res[7] = 0x01   # ANCOUNT = answer RRs = 1
        res[8] = 0x00
        res[9] = 0x00   #NSCOUNT = authority RRs = 0
        res[10] = 0x00
        res[11] = 0x00  # ARCOUNT = additional RRs = 0
        i = 0
        for i in range(12, 16 + self.name_length):  # 16 is because for TYPE and CLASS field(4 bytes)
            res[i] = Msg_arr[i]
        # after query name(and QTYPE & QCLASS)
        # now it's answer field:
        indx = 16 + self.name_length    # (right range not included)
        res[indx] = 0xc0    
        indx += 1
        res[indx] = 0x0c      # NAME(work as a pointer and points to the query name)
        indx += 1
        res[indx] = 0x00
        indx += 1
        res[indx] = 0x01    # TYPE: A
        indx += 1
        res[indx] = 0x00
        indx += 1
        res[indx] = 0x01    # CLASS = IN
        indx += 1
        res[indx] = 0x00
        indx += 1
        res[indx] = 0x00
        indx += 1
        res[indx] = 0x00
        indx += 1
        res[indx] = 0x13    # TTL = 19 seconds or 0 second
        indx += 1
        res[indx] = 0x00
        indx += 1
        res[indx] = 0x04    # DATA LENGTH = 4
        indx += 1

        if not Intercepted:
            for i in ip.split('.'):
                res[indx] = int(i)
                print(res[indx])
                indx += 1
        else:   # 0.0.0.0
            for i in range(4):
                res[indx] = 0x00
                indx += 1
        return bytes(res)

   

if __name__ == '__main__':
    cache_file = 'example.txt'
    #name_server=('202.141.160.1',53)
    name_server = ('223.5.5.5', 53)
    relay_server = DNS_Relay_Server(cache_file,name_server)   #构造一个DNS_Relay_Server实例
    relay_server.run()