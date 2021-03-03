%define brtd_version %(echo $brtd_RPM_VERSION)
%define rpm_release %(echo $RPM_RELEASE)
%define rpm_patch %(echo $RPM_PATCH)
%define _prefix /opt/ripple
Name:           brtd
# Dashes in Version extensions must be converted to underscores
Version:        %{brtd_version}
Release:        %{rpm_release}%{?dist}%{rpm_patch}
Summary:        brtd daemon

License:        MIT
URL:            http://ripple.com/
Source0:        brtd.tar.gz

BuildRequires:  cmake zlib-static ninja-build

%description
brtd

%package devel
Summary: Files for development of applications using xrpl core library
Group: Development/Libraries
Requires: openssl-static, zlib-static

%description devel
core library for development of standalone applications that sign transactions.

%prep
%setup -c -n brtd

%build
cd brtd
mkdir -p bld.release
cd bld.release
cmake .. -G Ninja -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE=Release -DCMAKE_UNITY_BUILD_BATCH_SIZE=10 -Dstatic=true -DCMAKE_VERBOSE_MAKEFILE=ON -Dvalidator_keys=ON
cmake --build . --parallel --target brtd --target validator-keys -- -v

%pre
test -e /etc/pki/tls || { mkdir -p /etc/pki; ln -s /usr/lib/ssl /etc/pki/tls; }

%install
rm -rf $RPM_BUILD_ROOT
DESTDIR=$RPM_BUILD_ROOT cmake --build brtd/bld.release --target install -- -v
rm -rf ${RPM_BUILD_ROOT}/%{_prefix}/lib64/cmake/date
install -d ${RPM_BUILD_ROOT}/etc/opt/ripple
install -d ${RPM_BUILD_ROOT}/usr/local/bin
ln -s %{_prefix}/etc/brtd.cfg ${RPM_BUILD_ROOT}/etc/opt/ripple/brtd.cfg
ln -s %{_prefix}/etc/validators.txt ${RPM_BUILD_ROOT}/etc/opt/ripple/validators.txt
ln -s %{_prefix}/bin/brtd ${RPM_BUILD_ROOT}/usr/local/bin/brtd
install -D brtd/bld.release/validator-keys/validator-keys ${RPM_BUILD_ROOT}%{_bindir}/validator-keys
install -D ./brtd/Builds/containers/shared/brtd.service ${RPM_BUILD_ROOT}/usr/lib/systemd/system/brtd.service
install -D ./brtd/Builds/containers/packaging/rpm/50-brtd.preset ${RPM_BUILD_ROOT}/usr/lib/systemd/system-preset/50-brtd.preset
install -D ./brtd/Builds/containers/shared/update-brtd.sh ${RPM_BUILD_ROOT}%{_bindir}/update-brtd.sh
install -D ./brtd/bin/getbrtdInfo ${RPM_BUILD_ROOT}%{_bindir}/getbrtdInfo
install -D ./brtd/Builds/containers/shared/update-brtd-cron ${RPM_BUILD_ROOT}%{_prefix}/etc/update-brtd-cron
install -D ./brtd/Builds/containers/shared/brtd-logrotate ${RPM_BUILD_ROOT}/etc/logrotate.d/brtd
install -d $RPM_BUILD_ROOT/var/log/brtd
install -d $RPM_BUILD_ROOT/var/lib/brtd

%post
USER_NAME=brtd
GROUP_NAME=brtd

getent passwd $USER_NAME &>/dev/null || useradd $USER_NAME
getent group $GROUP_NAME &>/dev/null || groupadd $GROUP_NAME

chown -R $USER_NAME:$GROUP_NAME /var/log/brtd/
chown -R $USER_NAME:$GROUP_NAME /var/lib/brtd/
chown -R $USER_NAME:$GROUP_NAME %{_prefix}/

chmod 755 /var/log/brtd/
chmod 755 /var/lib/brtd/

chmod 644 %{_prefix}/etc/update-brtd-cron
chmod 644 /etc/logrotate.d/brtd
chown -R root:$GROUP_NAME %{_prefix}/etc/update-brtd-cron

%files
%doc brtd/README.md brtd/LICENSE
%{_bindir}/brtd
/usr/local/bin/brtd
%{_bindir}/update-brtd.sh
%{_bindir}/getbrtdInfo
%{_prefix}/etc/update-brtd-cron
%{_bindir}/validator-keys
%config(noreplace) %{_prefix}/etc/brtd.cfg
%config(noreplace) /etc/opt/ripple/brtd.cfg
%config(noreplace) %{_prefix}/etc/validators.txt
%config(noreplace) /etc/opt/ripple/validators.txt
%config(noreplace) /etc/logrotate.d/brtd
%config(noreplace) /usr/lib/systemd/system/brtd.service
%config(noreplace) /usr/lib/systemd/system-preset/50-brtd.preset
%dir /var/log/brtd/
%dir /var/lib/brtd/

%files devel
%{_prefix}/include
%{_prefix}/lib/*.a
%{_prefix}/lib/cmake/ripple

%changelog
* Wed Aug 28 2019 Mike Ellery <mellery451@gmail.com>
- Switch to subproject build for validator-keys

* Wed May 15 2019 Mike Ellery <mellery451@gmail.com>
- Make validator-keys use local brtd build for core lib

* Wed Aug 01 2018 Mike Ellery <mellery451@gmail.com>
- add devel package for signing library

* Thu Jun 02 2016 Brandon Wilson <bwilson@ripple.com>
- Install validators.txt

