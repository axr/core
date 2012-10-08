#/**********************************************************\
#
# Auto-Generated Plugin Configuration file
# for AXR
#
#\**********************************************************/

set(PLUGIN_NAME "AXR")
set(PLUGIN_PREFIX "AXR")
set(COMPANY_NAME "AXRProject")

# ActiveX constants:
set(FBTYPELIB_NAME AXRLib)
set(FBTYPELIB_DESC "AXR 1.0 Type Library")
set(IFBControl_DESC "AXR Control Interface")
set(FBControl_DESC "AXR Control Class")
set(IFBComJavascriptObject_DESC "AXR IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "AXR ComJavascriptObject Class")
set(IFBComEventSource_DESC "AXR IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 815deb0c-3457-509d-a633-c0c1db7a2c33)
set(IFBControl_GUID b79342c5-a9da-554f-ab3e-45955edcef9c)
set(FBControl_GUID be0e945d-ab33-572a-bbf1-387787ca661b)
set(IFBComJavascriptObject_GUID 0fc427c3-bda6-5fab-a0c1-5eb9127fdae8)
set(FBComJavascriptObject_GUID 115e1e43-c99b-547f-ae71-9e874bd60d1e)
set(IFBComEventSource_GUID d3427105-dcdd-5cc6-98f6-16c3ceafa2df)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 9fec19c9-4e1d-5445-aa86-015cb9619211)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID ba15997e-f7b6-50c1-aa57-4f6671a44e91)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "AXRProject.AXR")
set(MOZILLA_PLUGINID "axr.vg/AXR")

# strings
set(FBSTRING_CompanyName "AXR Project")
set(FBSTRING_PluginDescription "The rendering engine that powers the AXR platform.")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2012 AXR Project")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "AXR")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "AXR")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "AXR_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-axr")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
