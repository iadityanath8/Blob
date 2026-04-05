# BLOB – Lightweight C Glob Pattern Matcher

BLOB is a small and fast **glob-style pattern matching library written in C**.
It allows matching text strings against patterns using common wildcard syntax such as `*`, `?`, and character sets `[ ]`.

The implementation is **non-recursive**, **allocation-free**, and designed to be **lightweight and embeddable** in low-level projects such as CLI tools, file filters, or custom scripting systems.

---

## Features

* Wildcard pattern matching
* No dynamic memory allocation
* Non-recursive algorithm
* Constant memory usage
* Simple API
* Syntax error detection for malformed patterns

Supported pattern operators:

| Pattern  | Meaning                                |
| -------- | -------------------------------------- |
| `*`      | Match zero or more characters          |
| `?`      | Match exactly one character            |
| `[abc]`  | Match any character in the set         |
| `[a-z]`  | Match character range                  |
| `[!abc]` | Match any character **not** in the set |

---

## Example

```c
#include "blob.h"

int main() {
    B_Result r = BLB_blob("maina*", "mainajs");

    if (r == B_MATCHED) {
        printf("Matched\n");
    } else {
        printf("Not matched\n");
    }
}
```

Output:

```
Matched
```

---

## API

### `BLB_blob`

```c
B_Result BLB_blob(str pattern, str text);
```

Match a text string against a glob pattern.

Parameters:

| Parameter | Description         |
| --------- | ------------------- |
| `pattern` | Glob pattern string |
| `text`    | Text to test        |

Returns:

| Result                          | Meaning                    |
| ------------------------------- | -------------------------- |
| `B_MATCHED`                     | Pattern matches text       |
| `B_UNMATCHED`                   | Pattern does not match     |
| `B_SYNTAX_ERROR_UNCLOSED`       | Unclosed `[` character set |
| `B_SYNTAX_ERROR_RANGE_NO_START` | Invalid range start        |
| `B_SYNTAX_ERROR_RANGE_NO_END`   | Invalid range end          |
| `B_SYNTAX_ERROR_WRG_RANGE`      | Invalid character range    |
| `B_SYNTAX_NEGATE_ERR`           | Invalid `!` usage in set   |

---

## Pattern Examples

| Pattern      | Text        | Result |
| ------------ | ----------- | ------ |
| `*.c`        | `main.c`    | ✓      |
| `file?.txt`  | `file1.txt` | ✓      |
| `[abc].txt`  | `b.txt`     | ✓      |
| `[!abc].txt` | `d.txt`     | ✓      |
| `main*`      | `mainajs`   | ✓      |

---

## Algorithm

The matcher uses an **iterative backtracking approach**.

When encountering a `*`:

* The pattern position after `*` is saved
* The current text position is saved
* If a later mismatch occurs, the algorithm **backtracks**
  and retries matching with the `*` consuming one more character.

This avoids recursion and keeps the implementation **O(n)** in typical cases.

---

## Design Goals

* Tiny implementation
* Predictable performance
* Easy to embed in C projects
* No dependencies
* No heap allocation

---

## License

MIT License
