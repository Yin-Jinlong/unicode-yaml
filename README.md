# unicode-yaml

## 使用

基本

```c++
#define UYAML_USE_STATIC

#include <iostream>
#include <uyaml/uyaml.h>

using namespace std;
using namespace UYAML;

int main() {
    WNode root;
    auto &a = *root[L"a"];
    auto &b = *a[L"b"];
    b = true;
    auto r = root.Dump<1>(L"\n");
    wcout << r;
    return 0;
}
```

读取

<details>
<summary>
TIP
</summary>

读取时可能会有异常抛出

</details>

```c++
#define UYAML_USE_STATIC

#include <iostream>
#include <uyaml/uyaml.h>

using namespace std;
using namespace UYAML;

int main() {
    WNode root;
    root.Parse(L"a:\n  b: false");
    auto r = root.Dump<1>(L"\n");
    wcout << r;
    return 0;
}

```

类型转换

```c++
bool b=node.asBool(false);
auto b=node.as<MyClass>({});
```

自定义类型需要实现模板

- `C` 字符类型，`char`,`wchar_t`,`char8_t`,`char16_t`,`char32_t`
- `T` 自定义类型

```c++
template<>
struct converter<C, T> {
    static T to(const Node<c> &node, T def) {
        // TODO
    }
    static Node<C> from(T val) { return Node<C>(val); }
}
```
