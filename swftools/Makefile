# $NetBSD: Makefile,v 1.21 2014/10/02 09:31:55 thomasklausner Exp $

DISTNAME=	swftools-0.9.2
CATEGORIES=	graphics multimedia converters
MASTER_SITES=	http://www.swftools.org/

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://www.swftools.org/
COMMENT=	Collection of SWF manipulation and creation utilities
LICENSE=	gnu-gpl-v2

# missing fmax(3)
NOT_FOR_PLATFORM=	NetBSD-[0-4].*-* NetBSD-5.0*-*

GNU_CONFIGURE=		yes
USE_TOOLS+=		gmake yacc
USE_LANGUAGES=		c c++
USE_LIBTOOL=		yes

# png2swf already installed as part of ming>=0.4.3.
CONFIGURE_ENV+=		DISABLEPNG2SWF=true

.include "options.mk"

INSTALL_MAKE_FLAGS=	prefix=${DESTDIR}${PREFIX}
INSTALL_MAKE_FLAGS+=	mandir=${DESTDIR}/${PREFIX}/${PKGMANDIR}

BUILDLINK_INCDIRS.fontconfig=	include/fontconfig

# remove empty man page -- delete this target if it ever gets filled in
post-extract:
	${RM} ${WRKSRC}/src/swfrender.1

.include "../../fonts/fontconfig/buildlink3.mk"
.include "../../mk/jpeg.buildlink3.mk"
.include "../../graphics/giflib/buildlink3.mk"
.include "../../multimedia/ming/buildlink3.mk"
.include "../../devel/zlib/buildlink3.mk"
.include "../../mk/pthread.buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
