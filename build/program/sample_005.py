import impy

i = 0

def impy_draw():
    global i
    impy.Begin("test window", True, 0)
    
    impy.Text("hogehoge")
    impy.Text("日本語にこにこ")
    if impy.Button("押すとどうなる？"):
        i += 2
    if impy.Button("+10"):
        i += 10
    
    impy.Text(str(i))
    impy.End()