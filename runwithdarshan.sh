
if [[ -z $DARSHAN_BASE_DIR ]]; then
 echo "Setup Darshan first"
else
  DARSHAN_ENABLE_NONMPI=1 LD_PRELOAD="$DARSHAN_BASE_DIR/lib/libdarshan.so" $1
fi
