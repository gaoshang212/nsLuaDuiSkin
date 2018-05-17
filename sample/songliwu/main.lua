-- local debug=require ("DebugUI")





local app=UI.CApplicationUI.sharedInstance()

--app:setResourcePath(app:instancePath().."skin")

--debug.showDebugUI()
local Window=UI.CWindowUI

--app:setResourceZip("360Demo.zip")
local wnd=Window.create({res="install.xml",name="songliwu",x=0,y=0,w=508,h=418,type="popup"})
wnd:centerWindow()

wnd:findControl("editDir"):setText(UI.Nsis.installDir())

--wnd:setIcon(app:resourcePath()..'logo.ico')

nsis = {
    onProgress = function(progress)
        local control = wnd:findControl("slrProgress")
        control:setValue(progress)

        if(progress >= 100)
        then
            local tab = wnd:findControl("wizardTab")
            tab:selectItem(2)
        end
    end,

    -- test = function()
    --     local tab = wnd:findControl("wizardTab")
    --     tab:selectItem(2)
    -- end
}

function readFile(fileName)
    local f = assert(io.open(fileName,'r'))
    local content = f:read('*all')
    f:close()
    return content
end







