#ifdef MENU_SIZEPOSITION
        POPUP "Size && Position"
        BEGIN
#ifdef	A_SETX		
            MENUITEM "Set X",                       0000
#endif
#ifdef A_SETY
			MENUITEM "Set Y",  						0001
#endif			
#ifdef A_SETW			
            MENUITEM "Set Width",                   0002
#endif				
#ifdef A_SETH		
            MENUITEM "Set Height",                  0003
#endif				
#ifdef A_SETA			
            MENUITEM "Set Angle",                   0004
#endif				
			
        END
#endif
#ifdef MENU_PRIVATE	
        POPUP "Private values"
        BEGIN

            MENUITEM "Set value",                   0005
        END		
#endif		