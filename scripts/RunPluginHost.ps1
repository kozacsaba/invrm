# RunPluginHost.ps1

# Full path to your Plugin Host executable
$exePath = "$PSScriptRoot\..\build\juce\extras\AudioPluginHost\AudioPluginHost_artefacts\Debug\AudioPluginHost.exe"

if (-Not (Test-Path $exePath)) {
    Write-Host "AudioPluginHost.exe not found. Building it first..."

    cmake -S "$PSScriptRoot\..\submodules\juce" -B "$PSScriptRoot\..\build\juce" -DCMAKE_BUILD_TYPE=Release -DJUCE_BUILD_EXTRAS=ON -DJUCE_BUILD_EXAMPLES=OFF
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed"
        exit 1
    }

    cmake --build "$PSScriptRoot\..\build\juce" --target AudioPluginHost
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed"
        exit 1
    }
} else {
    Write-Host "AudioPluginHost.exe found. Skipping build."
}

# Launch it as a background process
Start-Process -FilePath $exePath

# Wait for 2 seconds to allow process to initialize
#Write-Host "Waiting 2 seconds for Plugin Host to initialise"
#Start-Sleep -Seconds 2
#Write-Host "Done waiting"

exit 0
