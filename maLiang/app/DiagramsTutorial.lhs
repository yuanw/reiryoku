> {-# LANGUAGE NoMonomorphismRestriction #-}
> {-# LANGUAGE FlexibleContexts          #-}
> {-# LANGUAGE TypeFamilies              #-}
>
> import Diagrams.Prelude
> import Diagrams.Backend.SVG.CmdLine
> minorKey = square 0.2 # scaleY 0.3
> centerKey :: Diagram B
> centerKey =    (  beside (r2 (1,-1)) (text "V" # fontSizeL 0.1 <> strutX 0.1) (text "⇧" # fontSizeL 0.03 <> strutX 0.1 )) `atop` square 0.2
> west :: Diagram B
> west =  text "H" # fontSizeL 0.03 <> vMinorKey
> east :: Diagram B
> east = text "L" # fontSizeL 0.03 <> vMinorKey
> row :: Diagram B
> vMinorKey = square 0.2 # scaleX 0.3
> row = hcat [west, centerKey, east]
> northKey :: Diagram B
> northKey = text "K" # fontSizeL 0.03 <>  minorKey
> southKey :: Diagram B
> southKey = text "J" # fontSizeL 0.03 <>  minorKey
> example :: Diagram B
> example = (hcat [strutX 0.2 # scaleX 0.3, northKey]) === ( hcat . map alignT $ [west, centerKey, east]) === (hcat [strutX 0.2 # scaleX 0.3, southKey])
> main = mainWith example
