{
  description = "A Nix-flake-based C/C++ development environment";

  inputs.nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0"; # stable Nixpkgs

  outputs =
    { self, ... }@inputs:

    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f:
        inputs.nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import inputs.nixpkgs { inherit system; };
          }
        );
    in
    {
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default =
            pkgs.mkShell.override
              {
                # Override stdenv in order to change compiler:
                # stdenv = pkgs.clangStdenv;
              }

              {
                NIXPKGS_ALLOW_UNSUPPORTED_SYSTEM = 1;

                packages =
                  with pkgs;
                  [
                    libGL

                    # X11 dependencies
                    xorg.libX11
                    xorg.libX11.dev
                    xorg.libXcursor
                    xorg.libXi
                    xorg.libXinerama
                    xorg.libXrandr

                    clang-tools
                    cmake
                    codespell
                    conan
                    cppcheck
                    doxygen
                    gtest
                    lcov
                    vcpkg
                    vcpkg-tool
                    valgrind

                    # raylib

                    gdb
                    lldb

                    # to test windows
                    # wine

                    prettierd

                    nil
                    nixfmt
                  ]
                  ++ [pkgsCross.mingwW64.stdenv.cc]
                  ++ (if system == "aarch64-darwin" then [ ] else [ gdb ]);
              };
        }
      );
    };
}
