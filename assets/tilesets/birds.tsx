<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="birds" tilewidth="44" tileheight="40" tilecount="1" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="1" type="bird">
  <properties>
   <property name="delay" type="float" value="0"/>
   <property name="keyframes" type="file" value="birds/bird.json"/>
   <property name="path" value=""/>
   <property name="time" type="float" value="5"/>
  </properties>
  <image width="44" height="40" source="birds/bird.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="40" height="40">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
     <property name="restitution" type="float" value="0"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
</tileset>
