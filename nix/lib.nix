# Builds the logos-chatsdk-module library
{ pkgs, common, src }:

let
  libExt = if pkgs.stdenv.hostPlatform.isDarwin then "dylib" else "so";
  liblogoschatLib = "liblogoschat.${libExt}";
  liblibchatLib = "liblibchat.${libExt}";
in

pkgs.stdenv.mkDerivation {
  pname = "${common.pname}-lib";
  version = common.version;
  
  inherit src;
  inherit (common) nativeBuildInputs buildInputs cmakeFlags meta env;
  
  postInstall = ''
    mkdir -p $out/lib
    
    # Copy liblogoschat library from source
    srcLib="$src/lib/${liblogoschatLib}"
    if [ ! -f "$srcLib" ]; then
      echo "Expected ${liblogoschatLib} in $src/lib/" >&2
      exit 1
    fi
    cp "$srcLib" "$out/lib/"
    
    # Copy liblibchat library from source
    srcLibChat="$src/lib/${liblibchatLib}" 
    if [ ! -f "$srcLibChat" ]; then
      echo "Expected ${liblibchatLib} in $src/lib/" >&2
      exit 1
    fi
    cp "$srcLibChat" "$out/lib/"
    
    # Copy the chatsdk module plugin from the installed location
    pluginFile="$out/lib/logos/modules/chatsdk_module_plugin.${libExt}"
    if [ ! -f "$pluginFile" ]; then
      echo "Error: No chatsdk_module_plugin.${libExt} found" >&2
      exit 1
    fi
    cp "$pluginFile" "$out/lib/"
    
    # Remove the nested structure we don't want
    rm -rf "$out/lib/logos" 2>/dev/null || true
    rm -rf "$out/share" 2>/dev/null || true
  '';
}
