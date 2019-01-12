<?xml version="1.0" encoding="UTF-8"?>
<tileset name="test_walls" tilewidth="64" tileheight="64" tilecount="4" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="wall">
  <properties>
   <property name="keyframes" type="file" value="test_wall1.json"/>
  </properties>
  <image width="64" height="64" source="test_wall1.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="64" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="1" type="wall">
  <properties>
   <property name="keyframes" type="file" value="test_wall2.json"/>
  </properties>
  <image width="64" height="64" source="test_wall2.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0">
    <properties>
     <property name="damage" type="int" value="1"/>
    </properties>
    <polygon points="0,0 64,0 0,64"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="2" type="wall">
  <properties>
   <property name="keyframes" type="file" value="test_wall3.json"/>
  </properties>
  <image width="64" height="64" source="test_wall3.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0">
    <properties>
     <property name="damage" type="int" value="1"/>
    </properties>
    <polygon points="0,0 64,0 64,64"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="3" type="wall">
  <properties>
   <property name="keyframes" type="file" value="test_wall4.json"/>
  </properties>
  <image width="64" height="64" source="test_wall4.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="64">
    <properties>
     <property name="damage" type="int" value="1"/>
    </properties>
    <polygon points="0,0 64,-64 64,0"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
