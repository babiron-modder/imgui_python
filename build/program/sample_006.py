import impy
from Crypto.Util.number import bytes_to_long
from Crypto.Util.number import long_to_bytes
from Crypto.Util.number import getPrime
from Crypto.Util.number import isPrime

p = getPrime(128)  #128ビットの素数を取得
q = getPrime(128)  #128ビットの素数を取得
m = "miso" #平文
e = 65537
n = 0
d = 0
phi_n = 0
c = 0
md = 0

def impy_draw():
    global p, q, m, e, n, d, phi_n, c, md

    impy.Begin("test window", True, 0)

    if impy.Button("p"):
        p = getPrime(128)
    impy.SameLine()
    impy.Text("p: " + str(p))
    
    if impy.Button("q"):
        q = getPrime(128)
    impy.SameLine()
    impy.Text("q: " + str(q))
    
    impy.Text("e: " + str(e))

    m = impy.InputText("平文", m, 100)

    if impy.Button("RSA"):
        n = p * q
        phi_n = (p - 1)*(q - 1)
        d = pow(e, -1, phi_n)
        c = pow(bytes_to_long(m.encode()), e, n)  #暗号文
    

    impy.Text("c: " + str(c))

    if impy.Button("Decode"):
        md = pow(c, d, n)  #平文
        md = long_to_bytes(md).decode()
    
    
    impy.Text("md: " + str(md))

    impy.End()