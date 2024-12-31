# libbms

Library with basic functionality for:

- binary arrays,
- math waveform functions and
- strings. 

## Make Debian Package

Dependencies: `build-essential`, `dbhelper`

### GPG key

To create GPG key:

```bash
gpg --default-new-key-algo rsa4096 --gen-key
```

or:

```bash
gpg --full-generate-key
```

for newer versions.

	Then list the key using: `gpg --list-secret-keys --keyid-format=long`

It will show something like:

```bash
/home/jrm/.gnupg/pubring.kbx
----------------------------
sec   rsa4096/2D781E48EA42ADC9 2024-12-31 [SC] [expires: 2026-12-31]
      B906B282BD7602516762B5452D781E48EA42ADC9
uid                 [ultimate] jorbam <jordeam@gmail.com>
```

The key is `2D781E48EA42ADC9` for this case.

put this key in `~/.devscripts`:

```bash
DEBSIGN_KEYID=2D781E48EA42ADC9
```

### Build the package

Run the command in package base directory:

```bash
dpkg-buildpackage -us -uc
```
