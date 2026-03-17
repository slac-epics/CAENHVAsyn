TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install) into <top>/db
# databases, templates, substitutions like this
#DB += xxx.db
DB += ai.template
DB += ao.template
DB += bi.template
DB += bo.template
DB += stringin.template
DB += stringout.template
DB += longin.template
DB += longout.template
DB += reconnection.db

DB += A1589-board.substitutions
DB += A1589-channel.substitutions
DB += A2519-board.substitutions
DB += A2519-channel.substitutions
DB += A2552-board.substitutions
DB += A2552-channel.substitutions
DB += A7030-board.substitutions
DB += A7030-channel.substitutions
DB += A7030DP-channel.substitutions
DB += SYx527.substitutions

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_template = <templatename>

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

