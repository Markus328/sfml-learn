{pkgs}: let
  # Create a C program that prints Hello World
  helloWorld = pkgs.writeText "hello.cpp" ''
    #include <iostream>

    int main (void)
    {
      std::cout << "hello, world!\n";
      return 0;
    }
  '';

  # A function that takes host platform packages
  crossCompileFor = hostPkgs:
  # Run a simple command with the compiler available
    hostPkgs.runCommandCC "hello-world-cross-test" {} ''
      # Wine requires home directory
      HOME=$PWD

      # Compile our example using the compiler specific to our host platform
      $CXX ${helloWorld} -o hello

      # Run the compiled program using user mode emulation (Qemu/Wine)
      # buildPackages is passed so that emulation is built for the build platform
      ${hostPkgs.stdenv.hostPlatform.emulator hostPkgs.buildPackages} hello > $out

      # print to stdout
      cat $out
    '';
in
  # Statically compile our example using the two platform hosts
  # rpi = crossCompileFor pkgs.pkgsCross.raspberryPi;
  crossCompileFor pkgs.pkgsCross.mingwW64
