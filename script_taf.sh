
# ROOT : need to set the environnment of ROOT in case of nohup usage.

source /Users/zelbitar/ilcsoft.v1/root-install/bin/thisroot.sh

# TAF : set the TAF environnment

source /Users/zelbitar/HEP_SOFT/taf.git/taf.origin/taf/Scripts/thistaf.sh

# EXECUTE TAF: Full Binary Path should be given

/Users/zelbitar/HEP_SOFT/taf.git/taf.origin/taf/bin/bin/TAF -q -run 405 -dsfp 10
