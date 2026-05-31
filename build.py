from __future__ import annotations

from scripts.build import BuildContext, cmake_build, cmake_configure, main, run_cmd


def build_haikal(ctx: BuildContext) -> None:
    haikal_build_dir = ctx.root_dir / "extern" / "haikal" / "build"
    haikal_source_dir = ctx.root_dir / "extern" / "haikal"

    if (haikal_source_dir / "CMakeLists.txt").exists():
        haikal_build_dir.mkdir(parents=True, exist_ok=True)
        cmake_configure(
            source_dir=haikal_source_dir,
            build_dir=haikal_build_dir,
            generator=ctx.generator,
            c_compiler=ctx.c_compiler,
            build_type="Debug",
        )
        cmake_build(haikal_build_dir)
        run_cmd([haikal_build_dir / "haikal.exe"], cwd=ctx.root_dir)
        return

    cmake_build(ctx.build_dir, target=ctx.project.name)
    run_cmd([ctx.exe_path], cwd=ctx.root_dir)


if __name__ == "__main__":
    main(
        project_name="haikal",
        test_target="haikal_test",
        test_exe_name="haikal_test.exe",
        hooks={
            "pre_build": build_haikal,
        },
        extra_clean_paths=(
            "extern/haikal/build",
        ),
    )
