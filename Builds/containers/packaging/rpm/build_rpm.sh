#!/usr/bin/env bash
set -ex

cd /opt/brtd_bld/pkg
cp -fpu brtd/Builds/containers/packaging/rpm/brtd.spec .
cp -fpu brtd/Builds/containers/shared/update_sources.sh .
source update_sources.sh

# Build the rpm

IFS='-' read -r brtd_RPM_VERSION RELEASE <<< "$brtd_VERSION"
export brtd_RPM_VERSION

RPM_RELEASE=${RPM_RELEASE-1}

# post-release version
if [ "hf" = "$(echo "$RELEASE" | cut -c -2)" ]; then
    RPM_RELEASE="${RPM_RELEASE}.${RELEASE}"
# pre-release version (-b or -rc)
elif [[ $RELEASE ]]; then
    RPM_RELEASE="0.${RPM_RELEASE}.${RELEASE}"
fi

export RPM_RELEASE

if [[ $RPM_PATCH ]]; then
    RPM_PATCH=".${RPM_PATCH}"
    export RPM_PATCH
fi

cd /opt/brtd_bld/pkg/brtd
if [[ -n $(git status --porcelain) ]]; then
    git status
    error "Unstaged changes in this repo - please commit first"
fi
git archive --format tar.gz --prefix brtd/ -o ../rpmbuild/SOURCES/brtd.tar.gz HEAD
# TODO include validator-keys sources
cd ..

source /opt/rh/devtoolset-8/enable

rpmbuild --define "_topdir ${PWD}/rpmbuild" -ba brtd.spec
rc=$?; if [[ $rc != 0 ]]; then
    error "error building rpm"
fi

# Make a tar of the rpm and source rpm
RPM_VERSION_RELEASE=$(rpm -qp --qf='%{NAME}-%{VERSION}-%{RELEASE}' ./rpmbuild/RPMS/x86_64/brtd-[0-9]*.rpm)
tar_file=$RPM_VERSION_RELEASE.tar.gz

cp ./rpmbuild/RPMS/x86_64/* ${PKG_OUTDIR}
cp ./rpmbuild/SRPMS/* ${PKG_OUTDIR}

RPM_MD5SUM=$(rpm -q --queryformat '%{SIGMD5}\n' -p ./rpmbuild/RPMS/x86_64/brtd-[0-9]*.rpm 2>/dev/null)
DBG_MD5SUM=$(rpm -q --queryformat '%{SIGMD5}\n' -p ./rpmbuild/RPMS/x86_64/brtd-debuginfo*.rpm 2>/dev/null)
DEV_MD5SUM=$(rpm -q --queryformat '%{SIGMD5}\n' -p ./rpmbuild/RPMS/x86_64/brtd-devel*.rpm 2>/dev/null)
SRC_MD5SUM=$(rpm -q --queryformat '%{SIGMD5}\n' -p ./rpmbuild/SRPMS/*.rpm 2>/dev/null)

RPM_SHA256="$(sha256sum ./rpmbuild/RPMS/x86_64/brtd-[0-9]*.rpm | awk '{ print $1}')"
DBG_SHA256="$(sha256sum ./rpmbuild/RPMS/x86_64/brtd-debuginfo*.rpm | awk '{ print $1}')"
DEV_SHA256="$(sha256sum ./rpmbuild/RPMS/x86_64/brtd-devel*.rpm | awk '{ print $1}')"
SRC_SHA256="$(sha256sum ./rpmbuild/SRPMS/*.rpm | awk '{ print $1}')"

echo "rpm_md5sum=$RPM_MD5SUM" >  ${PKG_OUTDIR}/build_vars
echo "dbg_md5sum=$DBG_MD5SUM" >> ${PKG_OUTDIR}/build_vars
echo "dev_md5sum=$DEV_MD5SUM" >> ${PKG_OUTDIR}/build_vars
echo "src_md5sum=$SRC_MD5SUM" >> ${PKG_OUTDIR}/build_vars
echo "rpm_sha256=$RPM_SHA256" >> ${PKG_OUTDIR}/build_vars
echo "dbg_sha256=$DBG_SHA256" >> ${PKG_OUTDIR}/build_vars
echo "dev_sha256=$DEV_SHA256" >> ${PKG_OUTDIR}/build_vars
echo "src_sha256=$SRC_SHA256" >> ${PKG_OUTDIR}/build_vars
echo "brtd_version=$brtd_VERSION" >> ${PKG_OUTDIR}/build_vars
echo "rpm_version=$brtd_RPM_VERSION" >> ${PKG_OUTDIR}/build_vars
echo "rpm_file_name=$tar_file" >> ${PKG_OUTDIR}/build_vars
echo "rpm_version_release=$RPM_VERSION_RELEASE" >> ${PKG_OUTDIR}/build_vars

