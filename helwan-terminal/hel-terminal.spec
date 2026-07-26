Name:           helwan-terminal
Version:        0.1.5
Release:        1%{?dist}
Summary:        Helwan Terminal

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
meson setup builddir \
    --prefix=%{_prefix} \
    -Ddistro=redhat

meson compile -C builddir

%install
rm -rf %{buildroot}
DESTDIR=%{buildroot} meson install -C builddir

%files
%license LICENSE
%{_bindir}/helwan-terminal
%{_datadir}/applications/helwan-terminal.desktop
%{_datadir}/glib-2.0/schemas/helwan-terminal.gschema.xml
%{_datadir}/icons/hicolor/64x64/apps/helwan-terminal.png
%{_datadir}/helwan-terminal/helwan-commands.sh

%post
if [ -x /usr/bin/glib-compile-schemas ]; then
    glib-compile-schemas %{_datadir}/glib-2.0/schemas
fi

%postun
if [ -x /usr/bin/glib-compile-schemas ]; then
    glib-compile-schemas %{_datadir}/glib-2.0/schemas
fi

%changelog
* Tue Jul 21 2026 Saeed Badreldin <helwanlinux@gmail.com> - 0.1.5-1
- Initial RPM package
