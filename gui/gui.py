#!/usr/bin/env python
# -*- coding: utf-8 -*-
# generated by wxGlade 0.6.3 on Mon Feb 20 20:35:18 2012

import wx

# begin wxGlade: extracode
# end wxGlade



class InitFrame(wx.Frame):
    def __init__(self, *args, **kwds):
        # begin wxGlade: InitFrame.__init__
        kwds["style"] = wx.DEFAULT_FRAME_STYLE
        wx.Frame.__init__(self, *args, **kwds)
        self.frameStatusBar = self.CreateStatusBar(1, 0)
        
        # Tool Bar
        self.frameToolbar = wx.ToolBar(self, -1, style=wx.TB_HORIZONTAL|wx.TB_FLAT)
        self.SetToolBar(self.frameToolbar)
        self.frameToolbar.AddLabelTool(wx.NewId(), "wa", wx.Bitmap("/home/cfezequiel/Projects/thesis/gui/images/waypoint.png", wx.BITMAP_TYPE_ANY), wx.NullBitmap, wx.ITEM_NORMAL, "Add a waypoint", "")
        self.frameToolbar.AddLabelTool(wx.NewId(), "wd", wx.Bitmap("/home/cfezequiel/Projects/thesis/gui/images/nowaypoint.png", wx.BITMAP_TYPE_ANY), wx.NullBitmap, wx.ITEM_NORMAL, "Delete a waypoint", "")
        self.frameToolbar.AddSeparator()
        self.frameToolbar.AddLabelTool(wx.NewId(), "oa", wx.Bitmap("/home/cfezequiel/Projects/thesis/gui/images/obstacle.png", wx.BITMAP_TYPE_ANY), wx.NullBitmap, wx.ITEM_NORMAL, "Add an obstacle", "")
        self.frameToolbar.AddLabelTool(wx.NewId(), "od", wx.Bitmap("/home/cfezequiel/Projects/thesis/gui/images/noobstacle.png", wx.BITMAP_TYPE_ANY), wx.NullBitmap, wx.ITEM_NORMAL, "Delete an obstacle", "")
        self.frameToolbar.AddSeparator()
        self.frameToolbar.AddLabelTool(wx.NewId(), "send", wx.Bitmap("/home/cfezequiel/Projects/thesis/gui/images/send.gif", wx.BITMAP_TYPE_ANY), wx.NullBitmap, wx.ITEM_NORMAL, "Send commands", "")
        # Tool Bar end
        self.bmArea = wx.StaticBitmap(self, -1, wx.Bitmap("/home/cfezequiel/Projects/thesis/gui/images/area.png", wx.BITMAP_TYPE_ANY))

        self.__set_properties()
        self.__do_layout()
        # end wxGlade

    def __set_properties(self):
        # begin wxGlade: InitFrame.__set_properties
        self.SetTitle("Path planning")
        self.SetSize((772, 732))
        self.frameStatusBar.SetStatusWidths([-1])
        # statusbar fields
        frameStatusBar_fields = ["TODO"]
        for i in range(len(frameStatusBar_fields)):
            self.frameStatusBar.SetStatusText(frameStatusBar_fields[i], i)
        self.frameToolbar.Realize()
        # end wxGlade

    def __do_layout(self):
        # begin wxGlade: InitFrame.__do_layout
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.bmArea, 2, wx.ALL|wx.ALIGN_CENTER_HORIZONTAL|wx.ALIGN_CENTER_VERTICAL|wx.SHAPED|wx.FIXED_MINSIZE, 0)
        self.SetSizer(sizer)
        self.Layout()
        # end wxGlade

# end of class InitFrame


if __name__ == "__main__":
    app = wx.PySimpleApp(0)
    wx.InitAllImageHandlers()
    frame = InitFrame(None, -1, "")
    app.SetTopWindow(frame)
    frame.Show()
    app.MainLoop()