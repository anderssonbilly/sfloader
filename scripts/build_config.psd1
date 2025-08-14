@{
    defaultTask = "sfloader"
    command = "wcl386"

    fileName = "sfloader.exe"
    srcPath = ".\\src\\"
    objPath = ".\\build\\obj\\"
    outPath = ".\\build\\bin\\"
    logPath = ".\\build\\logs\\"
    compilerArgs = @(
        "-bcl=nt"
    )

    overrides = @{
        aweman32 = @{
            fileName = "AWEMAN32.dll"
            srcPath = ".\\test\\mocks\\"
            objPath = ".\\build\\obj\\mocks\\"
            outPath = ".\\build\\bin\\mocks\\"
            compilerArgs = @(
                "-bd",
                "-l=nt_dll",
                "-'option eliminate export AWEManager'"
            )
        }
    }
}
