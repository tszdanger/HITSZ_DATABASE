#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
    Function:常用对话框实例
    Input：NONE
    Output: NONE
    author: socrates
    blog:http://www.cnblogs.com/dyx1024/
    date:2012-07-07
'''

import wx


class MyFrame(wx.Frame):

    def __init__(self, parent, id):
        wx.Frame.__init__(self, parent, id, u'测试面板Panel', size=(600, 300))

        # 创建面板
        panel = wx.Panel(self)

        # 在Panel上添加Button
        button = wx.Button(panel, label=u'关闭', pos=(150, 60), size=(100, 60))

        # 绑定单击事件
        self.Bind(wx.EVT_BUTTON, self.OnCloseMe, button)

    #    #消息对话框
    #    def OnCloseMe(self, event):
    #        dlg = wx.MessageDialog(None, u"消息对话框测试", u"标题信息", wx.YES_NO | wx.ICON_QUESTION)
    #        if dlg.ShowModal() == wx.ID_YES:
    #            self.Close(True)
    #        dlg.Destroy()
    #
    def OnCloseMe(self, event):
        dlg = wx.TextEntryDialog(None, u"请在下面文本框中输入内容:", u"文本输入框标题", u"默认内容")
        if dlg.ShowModal() == wx.ID_OK:
            message = dlg.GetValue()  # 获取文本框中输入的值
            dlg_tip = wx.MessageDialog(None, message, u"标题信息", wx.OK | wx.ICON_INFORMATION)
            if dlg_tip.ShowModal() == wx.ID_OK:
                self.Close(True)
            dlg_tip.Destroy()
        dlg.Destroy()


if __name__ == '__main__':
    app = wx.App()
    frame = MyFrame(parent=None, id=-1)
    frame.Show()
    app.MainLoop()