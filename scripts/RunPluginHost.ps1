# Full path to your Plugin Host executable
$exePath = "$PSScriptRoot\..\build\juce\extras\AudioPluginHost\AudioPluginHost_artefacts\Release\AudioPluginHost.exe"

# Launch it as a background process
Start-Process -FilePath $exePath

# Wait for 2 seconds to allow process to initialize
echo "Waiting 2 seconds for Plugin Host to initialise"
Start-Sleep -Seconds 2
echo "Done waiting"

# Done — return to VS Code
exit 0