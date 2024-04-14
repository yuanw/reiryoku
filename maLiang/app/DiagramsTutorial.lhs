> {-# LANGUAGE NoMonomorphismRestriction #-}
> {-# LANGUAGE FlexibleContexts          #-}
> {-# LANGUAGE TypeFamilies              #-}
>
> import Diagrams.Prelude
> import Diagrams.Backend.SVG.CmdLine
> minorKey = square 0.2 # scaleY 0.3
> centerKey :: Diagram B
> centerKey = text "R" # fontSizeL 0.2 # fc white <> square 0.2
> west :: Diagram B
> west = vMinorKey
> east :: Diagram B
> east = vMinorKey
> row :: Diagram B
> vMinorKey = square 0.2 # scaleX 0.3
> row = hcat [west, centerKey, east]
> northKey :: Diagram B
> northKey = minorKey
> southKey :: Diagram B
> southKey =  minorKey
> example :: Diagram B
> example = (hcat [strutX 0.2 # scaleX 0.3, northKey]) === ( hcat . map alignT $ [west, centerKey, east]) === (hcat [strutX 0.2 # scaleX 0.3, southKey])
> main = mainWith example
