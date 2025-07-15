#brew install create-dmg

create-dmg \
--volname "Lester Installer" \
--volicon "macos_release_assets/lester.icns" \
--background "macos_release_assets/installer_background.png" \
--window-pos 200 120 \
--window-size 800 400 \
--icon-size 100 \
--icon "lester.app" 200 190 \
--hide-extension "lester.app" \
--app-drop-link 600 185 \
"Lester-Installer.dmg" \
"build_macos/bin/"