# DNS_Relay实验报告

## 数据结构

在`DNS_Relay_Server`类里，用字典`url_ip`存储配置文件里域名与IP地址的映射关系；

用字典`trans`存储不在配置文件里的请求报文

## 程序流程

1. `main`函数提供文件名、`name server`的IP地址和端口，调用`DNS_Relay_Server`的初始化函数：

```python
class DNS_Relay_Server:      #一个relay server实例，通过缓存文件和外部地址来初始化
    def __init__(self,cache_file,name_server):
        #url_IP字典:通过域名查询ID
        self.url_ip = {}
        self.cache_file = cache_file
        self.load_file()
        self.name_server = name_server
        #trans字典：通过DNS响应的ID来获得原始的DNS数据包发送方
        self.trans = {}    

......

if __name__ == '__main__':
    cache_file = 'example.txt'
    #name_server=('202.141.160.1',53)
    name_server = ('202.141.160.1', 53)
    relay_server = DNS_Relay_Server(cache_file,name_server)   #构造一个DNS_Relay_Server实例
    relay_server.run()
```

2. 调用`run`函数，蹲在53端口监听收到的数据，每次收到数据时就新开一个线程来处理数据：

```python
class DNS_Relay_Server:
    .......
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
```

3. 具体的处理函数是`handle`：

先通过`DNS_Packege`的初始化，将收到的报文进行分解（具体的见后面对`DNS_Packege`的分析）

通过被拆解报文的`QR`域判断这是请求报文还是响应报文：

* 若是请求报文，检查所请求的域名是否在配置文件里
  * 若在，直接根据配置文件的映射关系构造响应报文，并返回；
  * 若不在，将请求报文发给真正的DNS server
* 若是响应报文，将响应报文返回给本地

```python
class DNS_Relay_Server:
    ......
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
                        sent = server_socket.sendto(res, addr)
                        print('sent {} bytes back to {}'.format(sent, addr))
                    else:
                        print("Intercepted")
                        res = RecvDp.generate_response("0.0.0.0", True)
                        sent = server_socket.sendto(res, addr)
                        print('sent {} bytes back to {}'.format(sent, addr))
                else:
                    print("Not Found in the configuration file")
                    sent = server_socket.sendto(data, self.name_server)
                    print('sent {} bytes back to {}'.format(sent, self.name_server))
            else:               # response
                print("!!!!!!!response")
                local_addr = self.trans[RecvDp.ID]
                sent = server_socket.sendto(data, local_addr)
                print('sent {} bytes back to {}'.format(sent, local_addr))
```

4. 对请求报文的解析和构造响应报文：

根据DNS报文的相关规定进行构造，注意响应报文的域名是变长的，要用指针指示。

## 程序运行

