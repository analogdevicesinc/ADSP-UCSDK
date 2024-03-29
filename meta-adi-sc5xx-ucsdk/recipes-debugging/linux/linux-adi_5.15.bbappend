FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append = "\ 
            file://enable_osnoise.cfg \            
"
COMPATIBLE_MACHINE = "(adsp-sc573-ezkit|adsp-sc584-ezkit|adsp-sc589-ezkit|adsp-sc589-mini|adsp-sc594-som-ezkit|adsp-sc598-som-ezkit)"