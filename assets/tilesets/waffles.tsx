<?xml version="1.0" encoding="UTF-8"?>
<tileset version="1.2" tiledversion="1.2.1" name="waffles" tilewidth="352" tileheight="256" tilecount="11" columns="0">
 <grid orientation="orthogonal" width="1" height="1"/>
 <tile id="0" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle1.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="128" height="128" source="waffle1.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="128" height="128">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="1" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle2.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="256" height="256" source="waffle2.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="256" height="256">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <ellipse/>
   </object>
  </objectgroup>
 </tile>
 <tile id="2" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle3.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="128" height="128" source="waffle3.png"/>
  <objectgroup draworder="index">
   <object id="2" x="0" y="0" width="128" height="128">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="3" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle4.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="256" height="256" source="waffle4.png"/>
  <objectgroup draworder="index">
   <object id="1" x="0" y="0" width="256" height="256">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="5" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle6.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="128" height="128" source="waffle6.png"/>
  <objectgroup draworder="index">
   <object id="8" x="59" y="1">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 9,0 61,89 58,98 -46,98 -53,90"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="9" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle5.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="320" height="64" source="waffle5.png"/>
  <objectgroup draworder="index">
   <object id="2" x="0" y="0" width="320" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="11" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle7.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="128" height="192" source="waffle7.png"/>
  <objectgroup draworder="index">
   <object id="2" x="0" y="0" width="128" height="192">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="13" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle8.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="160" height="160" source="waffle8.png"/>
  <objectgroup draworder="index">
   <object id="1" x="16" y="16" width="128" height="128">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <ellipse/>
   </object>
   <object id="2" x="100" y="19">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 14,-11 16,6 32,1 29,17 46,17 39,34"/>
   </object>
   <object id="3" x="22" y="109">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 -8,12 8,13 6,31 21,24 24,43 39,32"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="16" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle9.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="192" height="192" source="waffle9.png"/>
  <objectgroup draworder="index">
   <object id="1" x="32" y="32" width="128" height="128">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <ellipse/>
   </object>
   <object id="2" x="79" y="160" width="32" height="32">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
  </objectgroup>
 </tile>
 <tile id="17" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle10.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="160" height="160" source="waffle10.png"/>
  <objectgroup draworder="index">
   <object id="1" x="16" y="16" width="128" height="128">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <ellipse/>
   </object>
   <object id="5" x="17" y="72">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 -14,-13 5,-17 -5,-34 12,-33"/>
   </object>
   <object id="6" x="30" y="39">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 -5,-19 12,-11"/>
   </object>
   <object id="7" x="121" y="29">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 15,-6 11,11 28,12 19,26"/>
   </object>
   <object id="8" x="141" y="55">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 15,6 3,18"/>
   </object>
   <object id="9" x="53" y="140">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 4,17 17,4 25,18 34,3"/>
   </object>
   <object id="10" x="89" y="144">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 12,14 15,-6"/>
   </object>
  </objectgroup>
 </tile>
 <tile id="18" type="obstacle">
  <properties>
   <property name="ambient_radius" type="float" value="5"/>
   <property name="ambient_sound" value=""/>
   <property name="keyframes" type="file" value="waffle11.json"/>
   <property name="orbit" type="bool" value="false"/>
   <property name="orbit_anchor" value=""/>
   <property name="orbit_ccw" type="bool" value="false"/>
   <property name="orbit_period" type="float" value="10"/>
   <property name="oscillate" type="bool" value="false"/>
   <property name="oscillate_angle" type="float" value="0"/>
   <property name="oscillate_distance" type="float" value="1"/>
   <property name="oscillate_period" type="float" value="10"/>
   <property name="oscillate_phase" type="float" value="0"/>
   <property name="pulsate" type="bool" value="false"/>
   <property name="pulsate_phase" type="float" value="0"/>
   <property name="pulsate_time_advanced" type="float" value="1"/>
   <property name="pulsate_time_receded" type="float" value="1"/>
   <property name="pulsate_time_transition" type="float" value="0.5"/>
   <property name="smash" type="bool" value="false"/>
   <property name="smash_anchor" value=""/>
   <property name="smash_fall_time" type="float" value="0.5"/>
   <property name="smash_hover_time" type="float" value="5"/>
   <property name="smash_phase" type="float" value="0"/>
   <property name="smash_recede_time" type="float" value="1"/>
   <property name="smash_sit_time" type="float" value="1"/>
   <property name="spin" type="bool" value="false"/>
   <property name="spin_ccw" type="bool" value="false"/>
   <property name="spin_period" type="float" value="10"/>
  </properties>
  <image width="352" height="64" source="waffle11.png"/>
  <objectgroup draworder="index">
   <object id="1" x="16" y="0" width="320" height="64">
    <properties>
     <property name="damage" type="int" value="1"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
   </object>
   <object id="5" x="17" y="62">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 -16,-5 0,-14 -16,-21 -1,-30"/>
   </object>
   <object id="6" x="17" y="4">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 -16,5 0,12 -16,21 -1,29"/>
   </object>
   <object id="7" x="337" y="5">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 14,4 -1,11 14,20 -1,27"/>
   </object>
   <object id="8" x="336" y="62">
    <properties>
     <property name="damage" type="int" value="2"/>
     <property name="repulsive" type="bool" value="true"/>
    </properties>
    <polygon points="0,0 14,-5 1,-14 15,-20 0,-31"/>
   </object>
  </objectgroup>
 </tile>
</tileset>
