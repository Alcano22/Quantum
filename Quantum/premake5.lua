project "Quantum"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Quantum/src",
        "%{wks.location}/Quantum/vendor"
    }

    filter "system:windows"
        systemversion "latest"

        defines "QTM_PLATFORM_WINDOWS"

    filter "configurations:Debug"
        defines "QTM_DEBUG"
        buildoptions "/MDd"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "QTM_RELEASE"
        buildoptions "/MD"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "QTM_DIST"
        buildoptions "/MD"
        runtime "Release"
        optimize "on"