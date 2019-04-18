<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="podge" tilewidth="64" tileheight="64" tilecount="1" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="podge">
  <properties>
    <property name="keyframes" type="file" value="podge/podge.json"/>
  </properties>
  <image width="64" height="64" source="podge/podge.png"/>
  <objectgroup draworder="index">
   <object id="3" x="0" y="0" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="false"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
</tileset>
