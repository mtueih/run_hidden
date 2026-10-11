add_rules("mode.debug", "mode.release")

set_project("run_hidden")
set_version("0.4.0")


-- lib.
target("run_hidden")
    set_kind("static")

    if is_plat("windows") then
        add_files("src/run_hidden_win32.c")
    end

    add_headerfiles("include/(run_hidden/run_hidden.h)")
    add_includedirs("include", {public = true})


-- cli.
for _, name in ipairs({"run_hidden", "run_hidden_wait"}) do
    target("cli_" .. name)
        set_kind("binary")
        set_basename(name)
        add_files("cli/main.c")
        add_deps("run_hidden")

        if is_plat("windows") then
            -- 编译为 Windows GUI 程序，以隐藏控制台窗口。
            add_rules("win.sdk.application")

            -- MSVC 编译器下，处理入口函数问题。
            add_ldflags("/ENTRY:mainCRTStartup", {tools = "link", force = true})
        end

        if name == "run_hidden_wait" then
            add_defines("RUN_HIDDEN_CLI_WAIT_MODE")
        end
end
