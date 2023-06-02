# Cmake

## find_package

```cmake
  find_package(<package> [version] [EXACT] [QUIET]
               [[REQUIRED|COMPONENTS] [components...]]
               [NO_POLICY_SCOPE])
```

查找并加载外来工程的设置。

该命令会设置<package>_FOUND变量，用来指示要找的包是否被找到了。如果这个包被找到了，与它相关的信息可以通过包自身记载的变量中得到。

QUIET选项将会禁掉包没有被发现时的警告信息。

REQUIRED选项表示如果报没有找到的话，cmake的过程会终止，并输出警告信息。

在REQUIRED选项之后，或者如果没有指定REQUIRED选项但是指定了COMPONENTS选项，在它们的后面可以列出一些与包相关的部件清单（components list）。

[version]参数需要一个版本号，它是正在查找的包应该兼容的版本号（格式是major[.minor[.patch[.tweak]]]）。

EXACT选项要求该版本号必须精确匹配。

如果在find-module内部对该命令的递归调用没有给定[version]参数，那么[version]和EXACT选项会自动地从外部调用前向继承。对版本的支持目前只存在于包和包之间（详见下文）。



eg:

```cmake
# add macros to generate ANTLR Cpp code from grammar
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
if(FLEX_FOUND)
  message(INFO "BISON FOUND")
endif()
```

