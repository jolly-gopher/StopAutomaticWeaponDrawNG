# CommonLibSSE NG Plugin Template

## Getting Started
Take the following steps to customize this template:

* Give the project a custom name in `CMakeLists.txt`.
* If your project will have public headers, the default assumes they will be located at `include/${PROJECT_NAME}`. You
  customize this by altering `PUBLIC_HEADER_DIR` in `CMakeLists.txt`.
* Change the name of `ProjectName.code-workspace` to match the name of your project.
* Change the name of `contrib/Distribution/Config/ProjectName.yaml` to match your project.
* Customize the metadata for your FOMOD in the XML files under `contrib/Distribution/fomod`.
* If your plugin will not use any Papyrus bindings, you can remove the `scripts-dev` feature from `vcpkg.json` to save
  time and space by not fetching/extracting Papyrus script sources.
* Test sources can be added to the `test` variable in `CMakeLists.txt`, which automatically triggers testing.