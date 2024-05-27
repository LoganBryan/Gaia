:: Check if the 'compiledShaders' folder exists - if not just make it
if not exist "%~dp0\compiledShaders\" mkdir %~dp0\compiledShaders

:: This could change if the VulkanSDK is somewhere else
C:/VulkanSDK/1.3.283.0/Bin/glslc.exe %~dp0/shaders/shader.vert -o %~dp0/compiledShaders/vert.spv
C:/VulkanSDK/1.3.283.0/Bin/glslc.exe %~dp0/shaders/shader.frag -o %~dp0/compiledShaders/frag.spv
pause