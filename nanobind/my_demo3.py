import demo3 
class App(demo3.Application):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
    
    def onOpen(self):
        print("Python App opened")
    
    def onClose(self):
        print("Python App closed")
    
    def onMessage(self, message):
        print("Python App received:", message)

app = App()
r = demo3.Runner(app)
r.run(1234)