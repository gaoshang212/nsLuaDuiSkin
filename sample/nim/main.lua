-- local debug=require ("DebugUI")





local app=UI.CApplicationUI.sharedInstance()

--app:setResourcePath(app:instancePath().."skin")

--debug.showDebugUI()
local Window=UI.CWindowUI

--app:setResourceZip("360Demo.zip")
local wnd=Window.create({res="install.xml",name="yongxin",x=0,y=0,w=508,h=418})
wnd:centerWindow()

nsis = {
    onProgress = function(progress)
        local control = wnd:findControl("install_progress")
        control:setValue(progress)
    end
}









