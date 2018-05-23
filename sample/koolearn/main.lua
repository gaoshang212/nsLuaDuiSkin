-- local debug=require ("DebugUI")
local app=UI.CApplicationUI.sharedInstance()

--app:setResourcePath(app:instancePath().."skin")

--debug.showDebugUI()
local Window=UI.CWindowUI

--app:setResourceZip("360Demo.zip")
local wnd=Window.create({res="skin.xml",name="新东方在线",x=0,y=0,w=800,h=650,type="popup"})
wnd:centerWindow()

--wnd:findControl("editDir"):setText(UI.Nsis.installDir())

--wnd:setIcon(app:resourcePath()..'logo.ico')

nsis = {
    onProgress = function(progress)
        print("1111111111111111111111111")
        local control = wnd:findControl("nsisProgress")
        control:setValue(progress)
       
        local cursor = wnd:findControl("progress_cursor")
        
        local left = math.ceil(4.66 * progress + 167)
        cursor:setPadding({x=left,y=0,w=0,h=0})

        local pvalue = wnd:findControl("progress_value")
        pvalue:setText(progress.."%");
        if(progress > 10)
        then
            pvalue:setPadding({x=-3,y=0,w=0,h=0})
        end

        if(progress >= 100)
        then
            pvalue:setPadding({x=-4,y=0,w=0,h=0})
        end
    end,

    finish = function()
        local tab = wnd:findControl("tab")
        tab:selectItem(2)
    end,
}







