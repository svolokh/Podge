<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="spikes" tilewidth="64" tileheight="64" tilecount="1" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="1" type="spike">
  <properties>
   <property name="keyframes" type="file" value="spike1.json"/>
   <property name="speed" type="float" value="10"/>
  </properties>
  <image width="64" height="64" source="spike1.png"/>
  <objectgroup draworder="index">
   <object id="4" x="0" y="12" width="40" height="40">
    <properties>
     <property name="damage" type="int" value="0"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <ellipse/>
   </object>
   <object id="5" x="29" y="15">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 33,16 0,35"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
