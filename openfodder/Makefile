# $NetBSD$

GITHUB_PROJECT=	openfodder
DISTNAME=	1.5
PKGNAME=	${GITHUB_PROJECT}-${DISTNAME}
CATEGORIES=	games
MASTER_SITES=	${MASTER_SITE_GITHUB:=OpenFodder/}
DIST_SUBDIR=	${GITHUB_PROJECT}

MAINTAINER=	pkgsrc-users@NetBSD.org
HOMEPAGE=	http://openfodder.com
COMMENT=	Engine for the Cannon Fodder game
LICENSE=	gnu-gpl-v3

WRKSRC=		${WRKDIR}/openfodder-${PKGVERSION_NOREV}
USE_LANGUAGES=	c++14

CXXFLAGS+=	-D OPENFODDER_DATADIR=\"${PREFIX}/share/\"
CXXFLAGS+=	-I${WRKSRC}/Source
CXXFLAGS+=	`sdl2-config --cflags`
LIBS+=		`sdl2-config --libs`
LIBS+=		-lSDL2_mixer
LIBS+=		${X11_LDFLAGS}

DEPENDS+=	openfodder-data-${DISTNAME}:../../wip/openfodder-data

FODDER_SRCDIRS=	Source Source/Amiga Source/PC Source/Structures
FODDER_SRCS=	${FODDER_SRCDIRS:=/*.cpp}

INSTALLATION_DIRS+=	bin share/doc/OpenFodder

do-build:
	${ECHO} 'const char* gitversion="pkgsrc-${PKGVERSION_NOREV}";' > ${WRKSRC}/Source/gitver.hpp
	cd ${WRKSRC} && ${CXX} ${CXXFLAGS} -o OpenFodder ${FODDER_SRCS} ${LIBS}

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/OpenFodder ${DESTDIR}${PREFIX}/bin
	${INSTALL_DATA} ${WRKSRC}/README.md ${DESTDIR}${PREFIX}/share/doc/OpenFodder/

.include "../../audio/SDL2_mixer/buildlink3.mk"
.include "../../devel/SDL2/buildlink3.mk"
.include "../../mk/x11.buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
