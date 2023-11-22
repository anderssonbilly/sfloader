param(
    [parameter(Mandatory = $true)][string]$name
)

$files = (Get-ChildItem -Path "./src" -Filter "*" -Recurse | Where-Object { $_.Extension.ToLowerInvariant() -in (".c", ".cpp", ".asm")} | Select-Object -ExpandProperty FullName | ForEach-Object { "{0}" -f $_ }  )

if(-not (Test-Path "./bin")) {
    New-Item "./bin" -ItemType Directory
}

if(-not (Test-Path "./obj")) {
    New-Item "./obj" -ItemType Directory
}

[string[]]$wclArguments = ("-bcl=nt", "-fo=./obj/", "-fe=./bin/$name.exe")
[string]$command = "wcl386"

$wclArguments += $files

if(Test-Path "$name.err") {
    Remove-Item "$name.err"
}

& $command $wclArguments

if(Test-Path "$name.err") {
    throw "Compile Error"
}