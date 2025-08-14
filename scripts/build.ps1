param(
    [parameter(Mandatory = $true)][string]$task,
    [parameter(Mandatory = $true)][string]$cfgPath
)

# Load build config file
$cfg = Import-PowerShellDataFile -Path $cfgPath

# Apply task-specific overrides
if ($cfg.overrides.ContainsKey($task)) {
    $override = $cfg.overrides[$task]
    foreach ($key in $override.Keys) {
        $cfg[$key] = $override[$key]
    }
} elseif ($task -ne $cfg["defaultTask"]) {
    Write-Error "Unknown task '$task'. Available tasks: $($cfg["defaultTask"]), $($cfg.overrides.Keys -join ', ')"
    exit 1
}

# Make sure output folders exist
foreach ($path in @($cfg["objPath"], $cfg["outPath"], $cfg["logPath"])) {
    if (-not (Test-Path $path)) {
        New-Item $path -ItemType Directory | Out-Null
    }
}

# Collect source files
$files = Get-ChildItem -Path $cfg["srcPath"] -Recurse -Include *.c | Select-Object -ExpandProperty FullName

# Build compiler/linker command and arguments
for ($i = 0; $i -lt $cfg["compilerArgs"].Count; $i++) {
    $cfg["compilerArgs"][$i] = $cfg["compilerArgs"][$i].Replace("'", "`"")
}
$arguments = @(
    "-fo=$($cfg["objPath"])",
    "-fe=$($cfg["outPath"])$($cfg["fileName"])",
    "-fr=$($cfg["logPath"]).log"
) + $cfg["compilerArgs"] + $files

# Run the compiler/linker
cmd.exe /c $cfg["command"] $arguments

# If logfile was created, move it to timestaped folder
$logFolder = Join-Path $cfg["logPath"] (Get-Date -Format "yyyy-MM-dd_HH-mm-ss")
foreach ($file in $files) {
    $logFile = Join-Path $cfg["logPath"] ((Get-Item $file).BaseName + ".log")
    if (Test-Path $logFile) {
        if (-not (Test-Path $logFolder)) {
            New-Item $logFolder -ItemType Directory | Out-Null
        }
        Move-Item $logFile $logFolder
    }
}
