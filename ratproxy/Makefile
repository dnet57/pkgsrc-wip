# $NetBSD: Makefile,v 1.4 2012/10/07 14:53:43 asau Exp $
#

DISTNAME=	ratproxy-1.51
PKGREVISION=	1
CATEGORIES=	security
MASTER_SITES=	http://ratproxy.googlecode.com/files/

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://code.google.com/p/ratproxy
COMMENT=	Passive web application security assessment tool
LICENSE=	apache-2.0

WRKSRC=		${WRKDIR}/ratproxy

GCC_REQD+=	3.0

LIBS+=		-lcrypto -lssl
LIBS.SunOS+=	-lsocket -lnsl

BINDIR=		${PREFIX}/bin
DATAROOTDIR=	${PREFIX}/share
DATADIR=	${DATAROOTDIR}/${PKGBASE}
DOCDIR=		${DATAROOTDIR}/doc/${PKGBASE}
REPLACE_SH=	ratproxy-report.sh

SUBST_CLASSES+=			fix-paths
SUBST_FILES.fix-paths=		ssl.c ratproxy-report.sh
SUBST_MESSAGE.fix-paths=	Performing pathname substitutions
SUBST_STAGE.fix-paths=		pre-build
SUBST_VARS.fix-paths=		DATADIR

do-install:
	${INSTALL_PROGRAM_DIR} ${DESTDIR}${BINDIR}
	${INSTALL_DATA_DIR} ${DESTDIR}${DATADIR}
	${INSTALL_DATA_DIR} ${DESTDIR}${DOCDIR}
	cd ${WRKSRC} && \
		${INSTALL_PROGRAM} ${WRKSRC}/ratproxy ${DESTDIR}${BINDIR} && \
		${INSTALL_DATA} ratproxy-back.png keyfile.pem messages.list \
			${DESTDIR}${DATADIR} && \
		${INSTALL_SCRIPT} ratproxy-report.sh \
			${DESTDIR}${BINDIR}/ratproxy-report
		${INSTALL_DATA} ${WRKSRC}/doc/README ${DESTDIR}${DOCDIR}

.include "../../security/openssl/buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
