Name:           helwan-terminal
Version:        0.1.3
Release:        1%{?dist}
Summary:        Helwan Terminal

License:        GPL-3.0-or-later
URL:            https://github.com/helwan-linux/helwan-terminal

BuildRequires:  gcc
BuildRequires:  meson
BuildRequires:  ninja-build
BuildRequires:  gtk3-devel
BuildRequires:  vte291-devel
BuildRequires:  glib2-devel

Requires:       gtk3
Requires:       vte291
Requires:       glib2

%description
Helwan Terminal is a GTK3 and VTE-based terminal emulator developed for Helwan Linux.

%prep

%build
meson setup builddir
meson compile -C builddir

%install
rm -rf %{buildroot}

install -Dm755 builddir/helwan-terminal \
    %{buildroot}%{_bindir}/helwan-terminal

install -Dm644 data/helwan-terminal.desktop \
    %{buildroot}%{_datadir}/applications/helwan-terminal.desktop

install -Dm644 data/helwan-terminal.gschema.xml \
    %{buildroot}%{_datadir}/glib-2.0/schemas/helwan-terminal.gschema.xml

install -Dm644 data/icons/hicolor/64x64/apps/helwan-terminal.png \
    %{buildroot}%{_datadir}/icons/hicolor/64x64/apps/helwan-terminal.png

%files
%license LICENSE
%{_bindir}/helwan-terminal
%{_datadir}/applications/helwan-terminal.desktop
%{_datadir}/glib-2.0/schemas/helwan-terminal.gschema.xml
%{_datadir}/icons/hicolor/64x64/apps/helwan-terminal.png

%post
if [ -x /usr/bin/glib-compile-schemas ]; then
    glib-compile-schemas %{_datadir}/glib-2.0/schemas
fi

%postun
if [ -x /usr/bin/glib-compile-schemas ]; then
    glib-compile-schemas %{_datadir}/glib-2.0/schemas
fi

%changelog
* Tue Jul 21 2026 Saeed Badreldin <helwanlinux@gmail.com> - 0.1.3-1
- Initial RPM package
