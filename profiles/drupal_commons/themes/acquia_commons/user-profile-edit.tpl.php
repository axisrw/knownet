	<?php 
	
		unset($form['account']['#type']); 
		unset($form['picture']['#type']);
		unset($form['picture']['picture_delete']); 
		unset($form['xmpp_user']);
		unset($form['user_terms']);
		unset($form['mimemail']);
		unset($form['contact']);
		unset($form['user_terms']);
		unset($form['messaging']);
		unset($form['account']['mail']['#description']);
		unset($form['picture']['picture_upload']['#description']); 
	?>
	
	
	<div id="user-profile-form">
		<div class="panel-col-first">
			<div class="inside">
					  <div class="field"><?php print drupal_render($form['picture']); ?></div>
					   <div class="field"><?php print drupal_render($form['account']['mail']); ?></div>
					  	
			</div>
		</div>
		<div class="panel-col">
			<div class="inside">
				<div class="field"><?php print drupal_render($form['Work information']); ?></div>
				<div class="field"><?php print drupal_render($form['submit']); ?></div>	
			 </div>
		</div>
		<div class="panel-col-last">
			<div class="inside">
				<div class="field"><?php print drupal_render($form['Personal information']); ?></div>	
						 
			</div>	
		</div>
		
	 </div>
	
	<div>
	<?php
		 print drupal_render($form); // prints the username field
	?>
	</div>