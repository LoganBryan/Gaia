-- premake5.lua

workspace "Gaia"
    architecture "x64"
    configurations {"Debug", "Release"}
    startproject "Gaia"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

VULKAN_SDK = "C:/VulkanSDK/1.3.283.0"

IncludeDirs = {}
IncludeDirs["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDirs["Volk"] = "thirdparty/Include"
IncludeDirs["GLFW"] = "thirdparty/glfw-3.4.bin.WIN64/include"
IncludeDirs["GLM"] = "thirdparty/glm"
IncludeDirs["STB"] = "thirdparty/stb"
IncludeDirs["VMA"] = "thirdparty/vma"
IncludeDirs["TinyOBjLoader"] = "thirdparty/tinyobjloader"

LibraryDirs = {}
LibraryDirs["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDirs["GLFW"] = "thirdparty/glfw-3.4.bin.WIN64/lib-vc2022"

project "Gaia"
    location "src"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
        "src/shaders/**"
    }

    vpaths
    {
        ["Headers"] = {"src/**.h"},
        ["Sources"] = {"src/**.cpp"},
        ["Shaders"] = {"src/shaders/**"}
    }   

    includedirs
    {
        "%{IncludeDirs.VulkanSDK}",
        "%{IncludeDirs.Volk}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.GLM}",
        "%{IncludeDirs.STB}",
        "%{IncludeDirs.VMA}",
        "%{IncludeDirs.TinyOBjLoader}"
    }

    libdirs
    {
        "%{LibraryDirs.VulkanSDK}",
        "%{LibraryDirs.GLFW}"
    }

    links
    {
        "vulkan-1.lib",
        "glfw3.lib",
        "glfw3dll.lib"
    }
    
    filter "system:windows"
        systemversion "latest"
        defines
        {
            "GAIA_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "GAIA_DEBUG"
        runtime "Debug"
        symbols "on"
        
        postbuildcommands 
        {
            "{COPY} %{wks.location}src/assets %{cfg.buildtarget.directory}assets",
            "{COPY} %{wks.location}src/compiledshaders %{cfg.buildtarget.directory}compiledshaders"
        }

    filter "configurations:Release"
        defines { "NDEBUG" }
        runtime "Release"
        optimize "on"
        buildoptions {"/Ob0"}

        postbuildcommands 
        {
            "{COPY} %{wks.location}src/assets %{cfg.buildtarget.directory}assets",
            "{COPY} %{wks.location}src/compiledshaders %{cfg.buildtarget.directory}compiledshaders"
        }