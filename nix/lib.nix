# Builds the logos-chatsdk-module library
{ pkgs, common, src }:

pkgs.stdenv.mkDerivation {
  pname = "${common.pname}-lib";
  version = common.version;
  
  inherit src;
  inherit (common) nativeBuildInputs buildInputs cmakeFlags meta env;
  
  # Determine platform-specific library extension
  liblogoschatLib = if pkgs.stdenv.hostPlatform.isDarwin then "liblogoschat.dylib" else "liblogoschat.so";
  
  postInstall = ''
    mkdir -p $out/lib
    
    # Copy liblogoschat library from source
    srcLib="$src/lib/''${liblogoschatLib}"
    if [ ! -f "$srcLib" ]; then
      echo "Expected ''${liblogoschatLib} in $src/lib/" >&2
      exit 1
    fi
    cp "$srcLib" "$out/lib/"
    
    # Fix the install name of liblogoschat on macOS
    ${pkgs.lib.optionalString pkgs.stdenv.hostPlatform.isDarwin ''
      ${pkgs.darwin.cctools}/bin/install_name_tool -id "@rpath/''${liblogoschatLib}" "$out/lib/''${liblogoschatLib}"
    ''}
    
    # Copy the chatsdk module plugin from the installed location
    if [ -f "$out/lib/logos/modules/chatsdk_module_plugin.dylib" ]; then
      cp "$out/lib/logos/modules/chatsdk_module_plugin.dylib" "$out/lib/"
      
      # Fix the plugin's reference to liblogoschat on macOS
      ${pkgs.lib.optionalString pkgs.stdenv.hostPlatform.isDarwin ''
        # Find what liblogoschat path the plugin is referencing and change it to @rpath
        for dep in $(${pkgs.darwin.cctools}/bin/otool -L "$out/lib/chatsdk_module_plugin.dylib" | grep liblogoschat | awk '{print $1}'); do
          ${pkgs.darwin.cctools}/bin/install_name_tool -change "$dep" "@rpath/''${liblogoschatLib}" "$out/lib/chatsdk_module_plugin.dylib"
        done
      ''}
    elif [ -f "$out/lib/logos/modules/chatsdk_module_plugin.so" ]; then
      cp "$out/lib/logos/modules/chatsdk_module_plugin.so" "$out/lib/"
    else
      echo "Error: No chatsdk_module_plugin library file found"
      exit 1
    fi
    
    # Remove the nested structure we don't want
    rm -rf "$out/lib/logos" 2>/dev/null || true
    rm -rf "$out/share" 2>/dev/null || true
  '';
}
