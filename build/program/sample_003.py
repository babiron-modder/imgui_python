print("load ok")
hoge = 'world'
i = 0
def impy_draw():
    global i
    print("hello " + str(hoge), i)
    i += 1
    return 0