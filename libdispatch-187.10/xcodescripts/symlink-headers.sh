if [ "$DEPLOYMENT_LOCATION" != YES ]; then
	DSTROOT="$CONFIGURATION_BUILD_DIR"
	[ -L "$DSTROOT$PRIVATE_HEADERS_FOLDER_PATH"/private.h ] && exit
fi

mv "$DSTROOT$PRIVATE_HEADERS_FOLDER_PATH"/private.h \
		"$DSTROOT$PRIVATE_HEADERS_FOLDER_PATH"/dispatch.h
ln -sf dispatch.h "$DSTROOT$PRIVATE_HEADERS_FOLDER_PATH"/private.h
