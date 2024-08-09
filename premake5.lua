workspace "Quantum"
    architecture "x64"
    startproject "Quantum"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags { "MultiProcessorCompile" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    group "Core"
        include "Quantum"
    group ""

    group "Dependencies"
        include "Quantum/vendor/GLFW"
        include "Quantum/vendor/Glad"
    group ""