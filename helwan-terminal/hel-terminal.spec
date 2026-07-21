Name:           hel-terminal
Version:        0.1.3
Release:        1%{?dist}
Summary:        A powerful and customizable terminal emulator for Helwan Linux

License:        GPL-3.0-or-later
URL:            https://github.com/helwan-linux/helwan-terminal
Source0:        %{name}-%{version}.tar.gz

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
%autosetup

%build
%meson
%meson_build

%install
%meson_install

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
