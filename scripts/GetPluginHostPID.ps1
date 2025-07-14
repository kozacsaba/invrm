$proc = Get-Process | Where-Object { $_.ProcessName -eq "AudioPluginHost" } | Select-Object -First 1
if ($proc) {
    Set-Content -Path "$PSScriptRoot\PluginHost.pid" -Value $proc.Id
    exit 0
} else {
    Write-Host "AudioPluginHost.exe not running"
    exit 1
}
