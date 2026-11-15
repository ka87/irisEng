/* Which FreeType modules this build contains. Not an upstream file:
   FreeType reads it through FT_CONFIG_MODULES_H, which is what that
   macro is for, so the vendored tree below stays untouched.

   Five modules, because the engine renders text from one TrueType
   face and wants it antialiased:

     autofit   the autohinter, so small sizes stay legible
     tt        the TrueType driver
     psnames   glyph names, which sfnt and autofit both want
     sfnt      the table reader every TrueType face goes through
     smooth    the antialiased rasterizer, which produces the
               8-bit coverage the atlas is built from

   Everything else upstream ships -- Type 1, CFF, CID, BDF, PCF,
   PFR, Windows FNT, SVG, SDF, the validators and the cache -- is
   absent from this file and from src/ with it. Adding one back means
   a line here and its sources beside the others. */

FT_USE_MODULE( FT_Module_Class, autofit_module_class )
FT_USE_MODULE( FT_Driver_ClassRec, tt_driver_class )
FT_USE_MODULE( FT_Module_Class, psnames_module_class )
FT_USE_MODULE( FT_Module_Class, sfnt_module_class )
FT_USE_MODULE( FT_Renderer_Class, ft_smooth_renderer_class )
