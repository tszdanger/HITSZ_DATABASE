import wx


class bucky(wx.Frame):

    # 结构函数，这样就没必要每次都要单独执行一下
    def __init__(self, parent, id):
        wx.Frame.__init__(self, parent, id, 'Frame aka window', size=(300, 200))

        # making a  panel
        panel = wx.Panel(self)  # self

        box = wx.TextEntryDialog(None, 'What\'s  your name', 'Titie', 'Default text')

        if box.ShowModal() == wx.ID_OK:  # 如果你点击了ok 我才会存储这个打进去的数值
            answer = box.GetValue()

        print(answer)


if __name__ == '__main__':
    app = wx.App()
    frame = bucky(parent=None, id=-1)
    frame.Show()
    app.MainLoop()  # 这个是用来执行这个 alpplication 的