<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="bump_bat" tilewidth="64" tileheight="64" tilecount="1" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="bump_bat">
  <properties>
   <property name="bounds" value=""/>
   <property name="keyframes" type="file" value="bump_bat.json"/>
  </properties>
  <image width="64" height="64" source="bump_bat.png"/>
  <objectgroup draworder="index">
   <object id="2" x="16" y="64">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
    </properties>
    <polygon points="0,0 -11,-11 -16,-33 -5,-56 17,-64 40,-52 48,-23 33,-1"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
